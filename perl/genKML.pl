#!/usr/bin/env perl

#
# Given a set of images, pull out the GPS information and generate a Google 
# Earth-formated KML.
#
# TODO
# - Support for multiple directories
# - Embed images, KMZ
# - Placemark icon as tiny thumbnail

use Image::ExifTool qw(ImageInfo);
use File::Basename;
use strict;

sub deg2dec($) {
  my ($in) = @_;

  chomp $in;
  $in =~ s/(deg|'|")//g;
  $in =~ s/ {2,}/ /g;

  my ($deg, $min, $sec, $sign) = split ' ', $in;

  if ($sign =~ /S/ || $sign =~ /W/) {
    $sign = -1;
  } else {
    $sign = 1;
  }

  return $sign*($deg+($min/60)+($sec/3600));
}

my @files;

if (!defined($ARGV[0])) {
  print "Requires at least one argument!\n";
  print "Usage: $0 <file|directory> [file ...]\n";
  exit;
}

if (-d $ARGV[0]) {
  opendir(DIR, $ARGV[0]) or die "Cannot open directory $ARGV[0]: $!";
  @files = readdir(DIR);
  closedir(DIR);
  chdir $ARGV[0];
} elsif (-f $ARGV[0]) {
  @files = @ARGV;
  chdir dirname($ARGV[0]);
}

my $KML = <<XML;
<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://www.opengis.net/kml/2.2" xmlns:gx="http://www.google.com/kml/ext/2.2" xmlns:kml="http://www.opengis.net/kml/2.2" xmlns:atom="http://www.w3.org/2005/Atom">
<Document>
	<name>Images.kml</name>
	<StyleMap id="msn_wht-pushpin0">
		<Pair>
			<key>normal</key>
			<styleUrl>#sn_wht-pushpin1</styleUrl>
		</Pair>
		<Pair>
			<key>highlight</key>
			<styleUrl>#sh_wht-pushpin</styleUrl>
		</Pair>
	</StyleMap>
	<Style id="sh_wht-pushpin">
		<IconStyle>
			<scale>1.3</scale>
			<Icon>
				<href>http://maps.google.com/mapfiles/kml/pushpin/wht-pushpin.png</href>
			</Icon>
			<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
		</IconStyle>
	</Style>
	<Style id="sn_wht-pushpin1">
		<IconStyle>
			<scale>1.1</scale>
			<Icon>
				<href>http://maps.google.com/mapfiles/kml/pushpin/wht-pushpin.png</href>
			</Icon>
			<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
		</IconStyle>
	</Style>
XML

foreach my $file (@files) {
  $file = basename($file);
  my $info = ImageInfo("$file", 'GPSLatitude', 'GPSLongitude');
  my $lat  = $$info{'GPSLatitude'};
  my $long = $$info{'GPSLongitude'};

  $lat  = deg2dec($lat);
  $long = deg2dec($long);

  if ($lat != 0 && $long != 0) {
    $KML .= <<POINT;
	<Placemark>
		<name>$file</name>
		<styleUrl>#msn_wht-pushpin0</styleUrl>
		<Point>
			<coordinates>$long,$lat,0</coordinates>
		</Point>
	</Placemark>
POINT
  }
}

$KML .= <<END;
</Document>
</kml>
END

print $KML;
