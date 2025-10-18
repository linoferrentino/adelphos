<?php

 // Set layout
$this->setLayout('layout.html.php')

?>


<h3>Login here in ἀδελφός </h3>

    <form action="/app/user/do_login"
          id="form-container" class="form" method="post" autocomplete="on">

<div class="form-row">
            <label for="userid">Userid </label>
            <input name="userid" type="text" id="userid"><br>
</div>
<div class="form-row">
            <label for"password">Password </label>
            <input  id="password" type="password" name="password"><br>
</div>
<div class="form-row">
	    <input type="submit" value="Login">
</div>
    </form>

