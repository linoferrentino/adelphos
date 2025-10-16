<?php
/**
 * @var \Slim\Views\PhpRenderer $this PhpRenderer instance
 * @var string $pageTitle
 * @var string $appName
 * @var string $name
 */

 // Set layout
$this->setLayout('layout.html.php')
?>

<h1><?= $pageTitle ?></h1>

<p class="test_style_inside">Here: <?= $appName ?>, <?= $name ?>!</p>
