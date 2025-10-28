<?php

 // Set layout
$this->setLayout('layout.html.php')

?>


<h4>Login here in ἀδελφός. If you have an invite code, click on the link instead or enter it here.</h4>

    <form action="/login"
          id="form-container" class="form" method="post" autocomplete="on">

            <label for="userid">User: </label>
            <input name="userid" type="text" id="userid" value='alice.smith@manhattan_01'/>
            <label for"password">Password: </label>
            <input id="password" type="password" name="password"/>
	    <input type="submit" value="Login" class="form-submit" />
    </form>

