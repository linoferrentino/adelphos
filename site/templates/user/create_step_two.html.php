<?php


 // Set layout
$this->setLayout('layout.html.php')
?>


<h3>Step 2, choose your currency.</h3>


 <form action="/user/do_register_step_3"
          id="select-currency-form" class="form-container" method="post" autocomplete="on">


 <label for="currencies">Your currency:</label>
        <select id="currencies" name="currency">
            <option value="dollar">Dollar</option>
            <option value="euro">Euro</option>
            <option value="yen">Yen</option>
            <option value="swiss">Swiss Franch</option>
            <option value="Pound">GBP Pound</option>
        </select>

        <button type="submit" class="form-submit">Next...</button>
</form>
<!-- <form action="/user/create_currency"
          id="select-currency-form" class="form-container" method="post" autocomplete="on"> -->
<button class="form-submit" onclick="alert('please email to adelphos@adelphos.it, I will add it');">My currency is not listed...</button>
<!--
</form> -->




