<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Register here in ἀδελφός </h3>

<p>Have you got an invite code? Join here or start a new trust net from the start.</p>


    <form action="/user/do_register"
          id="form-container" class="form" method="post" autocomplete="on">

<div class="form-row">
            <label for="userid">Userid</label>
            <input name="userid" type="text" id="userid">
</div>

<div class="form-row">
            <label for"password">Password</label>
            <input  id="password" type="password" name="password">
</div>

<div class="form-row">
            <label for"password">Repeat Password</label>
            <input  id="password_chk" type="password" name="password_chk">
</div>

<div class="form-row">
	    <label>

	    <input  name="join_type" type="radio" value="invite_code" onclick="toggleVisibility_all()">
 I have an invite code </label>

</div>
<div class="form-row">
	    <label>

            <input  name="join_type" type="radio" value="start_net" onclick="toggleVisibility_all()" checked>
I want to start a new net, my name in ἀδελφός will be user.family@level1
</label>

</div>



<div class="form-row" style="display:none;" id='invite_code_row'>
            <label for="invite_code" >Invite code:</label>
            <input name="invite_code" type="text" id="invite_code" >
</div>

<div id='rows_for_family' style="display:flex;" >

<div class="form-row" style="display:flex;" id='name_of_family_row'>
            <label for="invite_code" >Name of your family (level zero):</label>
            <input name="l0_name" type="text" id="l0_name" >
</div>

<div class="form-row" style="display:flex;" id='name_of_family_row'>
            <label for="invite_code" >Name of your friends' circle (level one):</label>
            <input name="l1_name" type="text" id="l1_name" >
</div>

</div>




<div class="form-row">
	    <input type="submit" value="Submit">
</div>

    </form>

<script>

function toggleVisibility(id) {
	const row = document.getElementById(id);
	if (row.style.display == 'none') {
		row.style.display = 'flex';
	} else {
		row.style.display = 'none';
	}

}

function toggleVisibility_all(id) {

	toggleVisibility('invite_code_row');
	toggleVisibility('rows_for_family');

}

</script>
