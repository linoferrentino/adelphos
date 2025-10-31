<?php

 // Set layout
$this->setLayout('layout.html.php')

?>


<h4>Login here in ἀδελφός. If you have an invite code, click <a href="user/got_invite"> here.</a></h4>

    <form action="/login"
          id="form-container" class="form-container" method="post" autocomplete="on">

<div class="form-row">
            <label class="form-label" for="userid">User: </label>
            <input class="form-input" name="userid" type="text" id="userid" value='alice.smith@manhattan_01'/>
</div>
<div class="form-row">
            <label class="form-label" for"password">Password: </label>
            <input class="form-input" id="password" type="password" name="password"/>
</div>
<div class="form-row">
	    <input type="submit" value="Login" class="form-submit" />
</div>
    </form>

