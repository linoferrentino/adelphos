<?php

 // Set layout
$this->setLayout('layout.html.php')

?>


<h4>Login here in ἀδελφός. If you have an invite code, click <a href="user/got_invite"> here.</a></h4>

<button class="form-submit" type="button" onclick="onHelp();">
Help.</button>

    <form action="/login"
          id="form-container" class="form-container" method="post" autocomplete="on">

<div class="form-row">
            <label class="form-label" for="userid">User: </label>
            <input class="form-input" name="userid" type="text" id="userid" />
</div>
<div class="form-row">
            <label class="form-label" for"password">Password: </label>
            <input class="form-input" id="password" type="password" name="password"/>
</div>
<div class="form-row">
	    <input type="submit" value="Login" class="form-submit" />
</div>
    </form>


<script>

document.addEventListener('DOMContentLoaded', function() {
   console.log('Document is fully loaded');
	const center_content=document.getElementById('center-content');
center_content.style = "display:none;";
	const bottom_content=document.getElementById('bottom-content');
bottom_content.style = "display:block;";


});


function onHelp() {
	const bottom_content=document.getElementById('bottom-content');
	bottom_content.style = "display:block;";
}


/*const center_content=document.getElementById('center-content');
center_content.style = "display:none;";*/
/*const bottom_content=document.getElementById('bottom-content');
bottom_content.style = "display:block;";*/
/*const help-frame=document.getElementById('help-frame');
help-frame.style = "display:block;";*/

</script>
