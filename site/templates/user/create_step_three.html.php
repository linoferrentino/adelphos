<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Register in ἀδελφός: Step 3, limits of trusts, see below..</h3>


    <form action="/user/do_register_step_4"
          id="form-container" class="form" method="post" autocomplete="off">

            <label for="userid">Trust for unreedemable money (tbel): </label>
            <input name="userid" type="text" id="userid" value='10'>
            <label for"family">Trust for reedemable money (tbel): </label>
            <input  id="family" type="text" name="family" value='20'>
            <button type="submit">Next...</button>

    </form>

