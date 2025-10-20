<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Register in ἀδελφός: Step 3, definition of initial trusts and commission, see help.</h3>


    <form action="/user/do_register_step_4"
          id="form-container" class="form" method="post" autocomplete="off">

<div class="form-row">
            <label for="userid">Trusts for unreedemable money (tbel):</label>
            <input name="userid" type="text" id="userid" value='10'>
</div>
<div class="form-row">
            <label for"family">Trust for reedemable money (tbel):</label>
            <input  id="family" type="text" name="family" value='20'>
</div>
<div class="form-row">
            <label for"interest">Interest for 90 days:</label>
            <input  id="interest" type="text" name="interest" value='0.5'>
</div>
<div class="form-row">
        <button type="submit">Next...</button>
</div>
    </form>

