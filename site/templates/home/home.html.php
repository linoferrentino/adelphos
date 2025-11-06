<?php

 // Set layout
$this->setLayout('layout.html.php')

?>



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


</script>
