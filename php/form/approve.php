<?php

include_once("stubs.php");
include_once("common.php");

$HEAD .= <<<HTML
<style type="text/css">
.selected {
	background-color: #ffc;
}
table {
	border:1px black solid;
	border-spacing:0;
	border-collapse:collapse;
}
table td {
	border:1px black solid;
	padding: 5px;
}
table td:hover {
	background-color: #66ccff;
}
</style>
</head>
HTML;

print $HEAD;

if (isset($_POST['submit'])) {
  process_form();
} else {
  create_form();
}

function process_form() {
  $entries = populate_entries();

  foreach ($entries as $entry) {
    $id = $entry['id'];

    if (in_array($id, $_POST['approve'])) {
      $query = "update entries_table set approved = true where id = ".$id;

      if (db_query($query)) {
        print "<p>Entries approved.</p>";
      } else {
        print "<p>DB error while attempting to approve entry</p>";
      }
    }
  }
}

function create_form() {
  print <<<HTML
  <script language="Javascript">
	// "select all"
	function Check(control, form, field) {
		chk = document.forms[form].elements[field];

		if (typeof(chk.checked) == "boolean") {
			chk.checked = control.checked;
		} else {
			for (i = 0; i < chk.length; i++) {
				chk[i].checked = control.checked;
			}
		}

		if (control.checked)
			$('#approve tr.row').addClass('selected');
		else
			$('#approve tr.row').removeClass('selected');
	}

  // function(s) for selecting checkbox by clicking within the row
  $(document).ready(function() {
      $('#approve tr.row')
      .filter(':has(:checkbox:checked)')
      .addClass('selected')
      .end()
      .click(function(event) {
        $(this).toggleClass('selected');
        if (event.target.type !== 'checkbox') {
        $(':checkbox', this).attr('checked', function() {
          return !this.checked;
          });
        }
        });
      });
  </script>
HTML;

  $entries = populate_entries();

  if (count($entries) == 0) {
    print "<strong>No unapproved entries!<strong>";
  } else {
    print <<<HTML
    <form method="POST" action="" name="approve_entries">
    <table id="approve" border="0" class="header">
    <tr>
      <td>Approve</td>
      <td>Parts</td>
      <td>Email</td>
      <td>Additional</td>
    </tr>
HTML;

    foreach ($entries as $entry) {
      print '
    <tr class="row">
      <td><input type="checkbox" name="approve[]" value="'.$entry['id'].'" /></td>
      <td>'.$entry['p1'].' '.$entry['p2'].'</td>
      <td>'.$entry['email'].'</td>
      <td>'.$entry['add'].'</td>
    </tr>';
    }

    print <<<HTML
    </table>
    <center>
    <label for="check_all">
      <input id="check_all" type="checkbox" name="check_all" value="yes" onClick="Check(this, 'approve_entries', 'approve[]')">
      Approve All
    </label>
    <br />
      <input type="submit" name="submit" value="Approve Selected">
    <center>
    </form>
HTML;
  }
}

function populate_entries() {
  $entries = array();
  $result = db_query("select id,p1,p2,email,add from entries_table where approved = false");

  while ($row = db_fetch_object($result)) {
    $entries[$row->id]['id'] = $row->id;
    $entries[$row->id]['p1'] = $row->p1;
    $entries[$row->id]['p2'] = $row->p2;
    $entries[$row->id]['email'] = $row->email;
    $entries[$row->id]['add'] = $row->add;
  }

  return $entries;
}

print $FOOT;

?>
