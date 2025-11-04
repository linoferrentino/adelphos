<?php


 // Set layout
$this->setLayout('layout.html.php')
?>


<h3>Step 2, choose your currency.</h3>


 <form action="/user/do_register_step_3"
          id="select-currency-form" class="form-container" method="post" autocomplete="on">

<div class="form-row">

 <label class='form-label' for="currencies">Your currency:</label>
        <select id="currencies" name="currency">
            <option value="dollar">Dollar</option>
            <option value="euro">Euro</option>
            <option value="yen">Yen</option>
            <option value="swiss">Swiss Franch</option>
            <option value="Pound">GBP Pound</option>
        </select>
</div>

<div class="form-row" style="justify-content: space-between;">

        <button type="submit" class="form-submit">Next...</button>

<button class="form-submit" type="button" onclick="alert('please email to adelphos@adelphos.it, I will add it')">My currency is not listed.</button>
</div>
</form>




