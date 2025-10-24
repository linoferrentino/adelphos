<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Step 1, choose your currency.</h3>


 <form action="/user/do_register_step_2"
          id="select-currency-form" class="form-container" method="post" autocomplete="on">


 <label for="currencies">Your currency:</label>
        <select id="currencies" name="currency">
            <option value="dollar">Dollar</option>
            <option value="euro">Euro</option>
            <option value="yen">Yen</option>
            <option value="swiss">Pound</option>
        </select>
        <button type="submit" class="form-submit">Next...</button>
</form>


