<?php

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">

  <title>ἀδελφός</title>
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
      <h2 class="test_style_inside">ἀδελφός</h2>
      <nav>
        <a href="/" >Home</a>
       <!-- <a href="/login/login/to" >Login</a>
        <a href="/dopay/393" >Pay</a> -->
        <a href="/user/register">Register</a>
        <a href="#">About</a>
      </nav>
    </aside>
    <main class="main-content">
      <header class="topbar">
        <h3>Welcome to the brotherhood network — share, connect, grow.
	      </header>

      <section class="topcontent">

    <?= $content ?>
<!--
    <iframe id="appFrame" src="https://www.adelphos.it/app/" frameborder="0"></iframe>
-->
      </section>

      <section class="content">

      <iframe id="helpFrame" src="https://www.adelphos.it/grav/<?= $help_page ?>" frameborder="0"></iframe>

      </section>
    </main>
  </div>

<footer>
  <p>&copy; 2025 Lino Ferrentino. ἀδελφός is licensed under GPLv3 <a href="mailto:info@adelphos.it">Info</a>
	<a href="https://github.com/linoferrentino/adelphos">Adelphos repository (github)</a>
</footer>



</body>
<script>
    function loadHelp(section) {
      const helpFrame = document.getElementById('helpFrame');

      helpFrame.src = 'https://www.adelphos.it/grav/' + section;
    }

</script>




</html>
