<?php



$this->setLayout('layout.html.php')
?>


<?php


	$invite_code = $bag['invite_code'] ?? null;

	if ($invite_code) {

		echo ("<h3> you have $invite_code</h3>");

	} else {

		echo ("<h3>Enter your invite code.</h3>");

	}


?>


