<?php

 // Set layout
$this->setLayout('layout.html.php')

?>


<h3>Login here in ἀδελφός </h3>
<h3>There is a demo user: alice.smith@manhattan_01, empty password</h3>

    <form action="/login"
          id="form-container" class="form" method="post" autocomplete="on">

<div class="form-row">
            <label for="userid">User</label>
            <input name="userid" type="text" id="userid" value='alice.smith@manhattan_01'><br>
</div>
<div class="form-row">
            <label for"password">Password </label>
            <input  id="password" type="password" name="password"><br>
</div>
<div class="form-row">
	    <input type="submit" value="Login">
</div>
    </form>

