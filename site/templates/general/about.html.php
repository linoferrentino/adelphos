<?php


 // Set layout
$this->setLayout('layout.html.php')
?>

<h3>Adelphos is free software.</h3>





<p>
Released under GPLv3 or any later version</p>
<p>
Copyright (c) Lino Ferrentino 2025 (lino.ferrentino@gmail.com)</p>

<script type="text/javascript" src="https://cdnjs.buymeacoffee.com/1.0.0/button.prod.min.js" data-name="bmc-button" data-slug="adelphos" data-color="#f9fbe7" data-emoji="🤑"  data-font="Comic" data-text="buy me a coffee, thanks!" data-outline-color="#000000" data-font-color="#000000" data-coffee-color="#FFDD00" ></script>

<script>

setTimeout(show_about, 10);

function show_about() {

   console.log('Document is fully loaded');
	/*const center_content=document.getElementById('center-content');
   center_content.style = "display:block;";*/
	const bottom_content=document.getElementById('bottom-content');
bottom_content.style += "left:2rem;top:250px;display:block;height:30vh;";

};

/*
document.addEventListener('DOMContentLoaded', 'show_about');
 */

/*
document.addEventListener('DOMContentLoaded', function() {
   console.log('Document is fully loaded');
	const center_content=document.getElementById('center-content');
center_content.style = "display:none;";
	const bottom_content=document.getElementById('bottom-content');
bottom_content.style = "display:block;";
bottom_content.top = "100px;";
});
 */

</script>

