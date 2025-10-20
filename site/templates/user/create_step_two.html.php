<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Register in ἀδελφός: Step 2, definition of level-zero, user and family</h3>


    <form action="/user/do_register_step_3"
          id="form-container" class="form" method="post" autocomplete="off">

<div class="form-row">
            <label for="userid">User:</label>
            <input name="userid" type="text" id="userid"><br>
</div>
<div class="form-row">
            <label for"family">Family:</label>
            <input  id="family" type="text" name="family"><br>
</div>
<div class="form-row">
        <button type="submit">Next...</button>
</div>
    </form>

