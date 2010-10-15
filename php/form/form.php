<?php

include_once("common.php");

$HEAD .= <<<HTML
<script type="text/javascript">
function toggle(id) {
  if (document.getElementById(id).style.display == "none") {
    value = "block";
  } else {
    value = "none";
  }

  document.getElementById(id).style.display = value;
}
$('textarea.limited').maxlength({
	'feeback' : '.charsLeft'
});
</script>
<style type="text/css">
table td:hover {
	background-color: gray;
}

table td.label:hover {
	background-color: transparent;
}
</style>
</head>
HTML;

print $HEAD:
?>

<body>

<form name="register" action="" method="post">
  <table id="form">
  <tr>
    <td class="label"><strong>Part One:</strong></td>
    <td><input type="text" name="p1" /></td>
    <td class="label"><strong>Part Two:</strong></td>
    <td><input type="text" name="p2" /></td>
  </tr>
  <tr>
    <td class="label"><strong>E-Mail:</strong></td>
    <td><input type="text" name="email" /></td>
  </tr>
  <tr>
    <td class="label"><strong>Additional:</strong></td>
    <td><input type="text" name="add" /></td>
  </tr>
  <tr>
    <td class="label"></td>
    <td class="label"><strong>Optional(s):</strong></td>
    <td class="label"><strong>Specify:</strong></td>
  </td>
  <?php
    $optionals = array("First", "Second", "Third", "Fourth", "Fifth");

    foreach ($optionals as $opt) {
      print '
      <tr>
        <td class="label"></td>
        <td><label for="'.$opt.'_check"><input id="'.$opt.'_check" type="checkbox"
          onClick="document.getElementById(\''.$opt.'\').disabled=!(this.checked)" />'.$opt.'</label></td>
          
        <td>
          <input disabled type="text" id="'.$opt.'" />
        </td>
      </tr>
      ';
    }
  ?>
  <tr>
    <td class="label"><strong>Confirm:</strong></td>
    <td><label for="yes"><input type="checkbox" id="yes" name="checkyes" />Yes</label></td>
  </tr>
  </table>
	<strong>Comments:</span>
	<br />
	<textarea maxlength="1000" rows=10 cols=50 name="area" class="limited"></textarea>
	<br />
  <input type="submit" name="submit" value="Register"/>
</form>

<?php

print $FOOT;

?>
