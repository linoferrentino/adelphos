<?php

/** @var \Odan\Session\FlashInterface $flash */
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">

  <title>ἀδελφός : the self-organizing, multi level, trust network. </title>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Serif&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="/styles.css">


<link rel="apple-touch-icon" sizes="180x180" href="/apple-touch-icon.png">
<link rel="icon" type="image/png" sizes="32x32" href="/favicon-32x32.png">
<link rel="icon" type="image/png" sizes="16x16" href="/favicon-16x16.png">
<link rel="manifest" href="/site.webmanifest">

</head>



<body>
  <div class="container">
    <aside class="sidebar">
      <nav>
	
        <a href="/" >Home</a>

<?php

// these are the menu which are dynamically generated
if (isset($menu_items)) {
	foreach ($menu_items as $menu) {
		echo("$menu\n");
	}
}


?>



        <a href="/about">About</a>

      </nav>
    </aside>
    <main class="main-content">
      <header class="top-content">

<?php

if (isset($bread_crumbs)) {

?>


<h4 class="test_style_inside">ἀδελφός, the multi-level, self-organizing
trust network.</h4> 

<?php

echo ("<div class=\"breadcrumb\">");
	$val = 0;
	foreach( $bread_crumbs as $key => $value) {
		if ($val != 0) {
			echo("<span>›</span>");
		} else {
			$val++;
		}
		echo("<a href=\"$value\">" . $key . "</a>");
	}
echo("</div>");
} else {
?>

<h2 class="test_style_inside">ἀδελφός, the multi-level, self-organizing
trust network.</h2> <h4>The site is in construction, with some demo data. 
It will be restarted periodically until 1st release (expected end Q1 2026)</h4> 


<?php
}


if (isset($flash)) {
foreach ($flash->all() ?? [] as $flashCategory => $flashMessages) {
	foreach ($flashMessages as $msg) {
		echo ("<div class=\"flahs-message flash-$flashCategory\">");
		echo ("<h3>" );
		if ($flashCategory == 'success' ) {
			echo ("🤗" );
		} else if ($flashCategory == 'error' ) {
			echo ("😤" );
		} else if ($flashCategory == 'info' ) {
			echo ("😏" );
		} else {
			echo ("😕" );

		}
		echo (" " . $msg . "</h3></div>");
	}
}
} else {
?>

      

<?php

}


?>


</header>


<?php




?>




      <div class="summary-scroll">

    <?= $content ?>

      </div>

      <div class="bottom-content">

      <iframe id="helpFrame" src="https://www.adelphos.it/grav/<?= $help_page ?>" style="border=none;" ></iframe>



      </div>
    </main>
  </div>

<footer>
  <p>&copy; 2025 Lino Ferrentino. ἀδελφός is licensed under GPLv3 <a href="mailto:info@adelphos.it">Info</a>
	<a href="https://github.com/linoferrentino/adelphos">Source code</a>
<hr>

<div class="bottom_debug_panel"/>

<p> Debug panel, just for development, please ignore </p>

<?= date(DATE_RFC2822);  ?>


<?php


	if (isset($session)) { 
		echo "<br> Parameters </br>";
		var_dump($session);
		echo "<br>";
	}

?>


</footer>



</body>



</html>
