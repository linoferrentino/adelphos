<?php


$this->setLayout('layout.html.php')
?>

<h3>Step 3, set your family equity (trust in the system)</h3>

<p> In adelphos trust and credit are linked: change one and the other will
follow.</p>

    <form action="/user/do_register_step_4"
          id="form-container" class="form-container" method="post" autocomplete="off">

<div class="form-row">
            <label class='form-label' for="userid"> Equity ($)  </label>
            <input class="form-input" name="userid" type="text" id="equity_cur" value='1'>
            <label class='form-label' for="userid"> Trust (τ-bel) </label>
            <input class="form-input" name="userid" type="text" id="equity_tbel" value='0'>
</div>

<div class="form-row">
            <label class='form-label' for"ratio-t0tn">Ratio τ₀/τ<sub>n</sub></label>
            <input  class="form-input" id="family" type="text" name="ratio-t0tn" value='20'>
</div>

<div class="form-row">
            <label class='form-label' for"family">Ratio τ<sub>n</sub>/τ<sub>∞</sub></label>
            <input  class="form-input" id="family" type="text" name="family" value='20'>
</div>


<div class="form-row">
            <button type="submit">Next...</button>
</div>

    </form>

<script>

const equit_cur = document.getElementById("equity_cur");
const equit_tbel = document.getElementById("equity_tbel");

equity_cur.addEventListener('input', (event) => {

	tbel_value = 10 * Math.log10(equity_cur.value);

	equity_tbel.value =  tbel_value;

});

equity_tbel.addEventListener('input', (event) => {

	cur_value = Math.pow(10, equity_tbel.value / 10);
	equity_cur.value = cur_value;

});



</script>
