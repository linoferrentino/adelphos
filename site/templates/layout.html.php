<?php

/** @var \Odan\Session\FlashInterface $flash */
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">

  <title>ἀδελφός : the self-organizing, fractal trust network. </title>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Serif&display=swap" rel="stylesheet">
<!--    <link rel="stylesheet" href="/styles.css"> -->
    <link rel="stylesheet" href="/assets/css/styles.css">


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
	$menu_items = $bag['menu_items'] ?? null;

	if (isset ($menu_items) ) {
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

$bread_crumbs = $bag['bread_crumbs'] ?? null;

if (isset($bread_crumbs)) {

?>


<h4 class="test_style_inside">ἀδελφός, the self-organizing, fractal
trust network.</h4> 

<?php

//echo ("<div class=\"breadcrumb-line\"> <div class=\"breadcrumb\">");
echo ("<div class=\"breadcrumb-line\"><div class=\"breadcrumb\">");
//echo ("<div class=\"breadcrumb\">");
	$val = 0;
	foreach( $bread_crumbs as $key => $value) {
		if ($val != 0) {
			echo("<span>›</span>");
		} else {
			$val++;
		}
		echo("<a href=\"$value\">" . $key . "</a>");
	}
//echo("</div><h3>help</h3></div>");
//echo("</div><button class=\"breadcrumb\" style='check' onclick='toggleHelp();'>help</h3></div>");
echo("</div><h3 class=\"breadcrumb\" onclick='toggleHelp();' id='get_help_id' >help</h3></div>");
//echo("</div>hello</div>");
//echo("</div></div>");
//echo("</div>");
} else {
?>

<h2 class="test_style_inside">ἀδελφός, the self-organizing, fractal 
trust network.</h2> <h4>The site is in construction, with some demo data. 
It will be restarted periodically until 1st release (expected end Q1 2026)</h4> 

<!-- style="border=none; -->

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

      
<!-- style="border=none; -->

<?php

}


?>

<!-- style="border=none; -->

</header>


<div class="overlay-container">


      <div class="center-content" id='center-content'>

    <?= $content ?>

      </div>

      <div class="bottom-content" id='bottom-content'>

<?php

	$help_page = $bag['help_page'] ?? null;
	if (isset($help_page)) {

?>
<!-- style="border=none; -->

      <iframe id="help-frame" src="https://www.adelphos.it/grav/<?= $help_page ?>"  ></iframe> 

<?php
	} else {

?>

<!-- style="border=none; -->

      <iframe id="help-frame" src="https://www.adelphos.it/grav/"  ></iframe>
<?php
	}
?>

      </div>
</div> <!--overlay-->


    </main>
  </div>

<footer>
  <p>&copy; 2025 Lino Ferrentino. ἀδελφός is licensed under GPLv3 <a href="mailto:info@adelphos.it">Info</a>
	<a href="https://github.com/linoferrentino/adelphos">Source code</a>
<hr>

<div class="bottom_debug_panel">

<?php

	$dbgvalue = $bag['_bag_debug_value'] ?? 'n';

	if ($dbgvalue == 'y') {


?>



<?php


		echo "<br> Debug info, please ignore: </br> <pre>";
		echo html(json_encode($bag, JSON_PRETTY_PRINT));

	} else {
		echo ("<pre> no debug " . $dbgvalue);
	}

	echo "</pre></div>";
?>


</footer>



</body>

<script>
function toggleHelp() {

	const get_help_id = document.getElementById('get_help_id');

	const bottom_content=document.getElementById('bottom-content');

	if (get_help_id.innerHTML == "Hide help") {
		get_help_id.innerHTML = "Help";
		bottom_content.style = "display:none;";
	} else {
		get_help_id.innerHTML = "Hide help";
		bottom_content.style = "display:block;";
	}



}
</script>


</html>
