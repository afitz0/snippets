<?php
  $showForm = true;

  if (isset($_POST['submit'])) {
    require_once "includes/formvalidator.php";

    print <<<HTML
      <style type="text/css">
        div.formError {
          color: red;
        }
      </style>
HTML;
    
    $validator = new FormValidator();
    $validator->addValidation("fname","req","Please fill in First Name");
    $validator->addValidation("lname","req","Please fill in Last Name");
    $validator->addValidation("email","email","The input for E-Mail should be a valid email address");
    $validator->addValidation("email","req","Please fill in Email");
    $validator->addValidation("employer","req","Please fill in Employer");
    
    if($validator->ValidateForm()) {

				$fname = $_POST['fname'];
				$lname = $_POST['lname'];
				$email = $_POST['email'];
				$employer = $_POST['employer'];
				$list = isset($_POST['list']) ? true : false;

				$query = "insert into eapf_members (fname,lname,email,employer,mailing_list)
					values (
					'".$fname."',
					'".$lname."',
					'".$email."',
					'".$employer."',
					".$list."
					)";

				db_query($query);

				if (db_error()) {
					print '<div class="formError">Error inserting into database.</div>';
				} else {
					print "<h2>Success!</h2>";
					print "<p>Thank you for registering with EAPF, ".$fname." ".$lname."!</p>";

					if ($list) {
						print "<p>You will be added to the eapf-members@eapf.org mailing list using the email you provided: ".$email.".</p>";
					}
				}

        $showForm = false;
    } else {
        print "<h2>Validation Errors</h2>";
        print "<strong>Please correct the errors below and submit again</strong>";

        $errorHash = $validator->GetErrors();
        foreach($errorHash as $fieldName => $error) {
            print "<div class=\"formError\">$error</div>\n";
        }
    }
  }

  if ($showForm) {
?>
<style type="text/css">
table {
  border: 0px;
}
table td {
  border: 0px;
}
table td.label:hover {
  background-color: transparent;
}
</style>

<script type="text/javascript">
function toggle(id) {
  if (document.getElementById(id).style.display == "none") {
    value = "block";
  } else {
    value = "none";
  }

  document.getElementById(id).style.display = value;
}
</script>
<form name="register" action="" method="post">
  <table id="form">
  <tr>
    <td class="label"><strong>First Name:</strong></td>
    <td><input type="text" name="fname" /></td>
    <td class="label"><strong>Last Name:</strong></td>
    <td><input type="text" name="lname" /></td>
  </tr>
  <tr>
    <td class="label"><strong>E-Mail:</strong></td>
    <td><input type="text" name="email" /></td>
  </tr>
  <tr>
    <td class="label"><strong>Employer:</strong></td>
    <td><input type="text" name="employer" /></td>
  </tr>
  <tr>
    <td class="label"></td>
    <td class="label"><strong>Sector(s):</strong></td>
    <td class="label"><strong>With whom:</strong></td>
  </td>
  <?php
    $jobAreas = array("Industry", "Researcher", "Educator", "Learner", "Student");

    foreach ($jobAreas as $job) {
      print '
      <tr>
        <td class="label"></td>
        <td><label for="'.$job.'_check"><input id="'.$job.'_check" type="checkbox"
          onClick="document.getElementById(\''.$job.'\').disabled=!(this.checked)" />'.$job.'</label></td>
          
        <td>
          <input disabled type="text" id="'.$job.'" />
        </td>
      </tr>
      ';
    }
  ?>
  <tr>
    <td class="label"><strong>Join mailing list:</strong></td>
    <td><label for="yes"><input type="checkbox" id="yes" name="list" />Yes</label></td>
  </tr>
  </table>
  <input type="submit" name="submit" value="Register"/>
</form>

<?php
  } // if showForm
?>
