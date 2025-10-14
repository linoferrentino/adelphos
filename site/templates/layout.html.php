<?php
/**
 * @var \Slim\Views\PhpRenderer $this PhpRenderer instance
 * @var string $content PHP-View var page content
 * @var string $appName
 * @var string $pageTitle
 */
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="assets/css/style.css">
    <title><?= $appName ?> <?= $pageTitle ? " - $pageTitle" : '' ?></title>
</head>
<body>
    <main><?= $content ?></main>
</body>
</html>
