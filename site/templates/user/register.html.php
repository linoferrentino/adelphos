<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Register in ἀδελφός: Step 1, tell me about yourself and your inner trust group: the family</h3>


    <form action="/user/do_register_step_2"
          id="reg_step_1" class="form-container" method="post" autocomplete="off">

<div class="form-row">
            <label class="form-label" for="userid">Your user name:</label>
            <input class="form-input" name="userid" type="text" id="userid">

</div>
<div class="form-row">
            <label class="form-label" for"family">Your family name:</label>
            <input class="form-input"  id="family" type="text" name="family">
</div>
<div class="form-row">
            <label class="form-label" for"email">Your email:</label>
            <input class="form-input"  id="email" type="text" name="email">
</div>
<div class="form-row">
            <label class="form-label" for"password">Your password:</label>
            <input class="form-input"  id="password_1" type="password" name="password">
</div>

<div class="form-row">
            <label class="form-label" for"password">Confirm password:</label>
            <input class="form-input"  id="password_2" type="password" name="password_2">
</div>
<div class="form-row">
            <button type="submit">Next...</button>
</div>
    </form>

