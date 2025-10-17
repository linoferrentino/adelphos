<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Register here in ἀδελφός </h3>

<p>Have you got an invite code? Join here or start a new trust net from the start.</p>


    <form action="/app/user/do_register"
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

            <input  name="join_type" type="radio" value="invite_code"> I have a code
</label>

	    <label>

            <input  name="join_type" type="radio" value="start_net" checked>
I want to start a new net
</label>

</div>



<div class="form-row">
	    <input type="submit" value="Submit">
</div>

    </form>
