<?php

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
       <!-- <a href="/login/login/to" >Login</a>
        <a href="/dopay/393" >Pay</a> -->
        <a href="/user/register">Register</a>
        <a href="/about">About</a>
      </nav>
    </aside>
    <main class="main-content">
      <header class="top-content">

      <h2 class="test_style_inside">ἀδελφός, the multi-level, self-organizing
trust network.</h2> <h4>The site is in construction, you are encouraged to use
the site, but all data might be erased before 1st release (expected February
2026)</h4> </header>

      <div class="middle-content">

    <?= $content ?>

      </div>

      <div class="bottom-content">

      <iframe id="helpFrame" src="https://www.adelphos.it/grav/<?= $help_page ?>" style="border=none;" ></iframe>



      </div>
    </main>
  </div>

<footer>
  <p>&copy; 2025 Lino Ferrentino. ἀδελφός is licensed under GPLv3 <a href="mailto:info@adelphos.it">Info</a>
	<a href="https://github.com/linoferrentino/adelphos">Adelphos repository (github)</a>
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

<!--
< ?php
for ($i = 1; ; $i++) {
    if ($i > 10) {
        break;
    }
    echo "<p>" . $i . "</p>";
}

? >

</div>

-->

<!--
<-?= $pars ?>
-->

</footer>



</body>
<script>
    function loadHelp(section) {
      const helpFrame = document.getElementById('helpFrame');

      helpFrame.src = 'https://www.adelphos.it/grav/' + section;
    }

</script>




</html>
