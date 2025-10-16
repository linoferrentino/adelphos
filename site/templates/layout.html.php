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
    <title><?= $appName ?> <?= $pageTitle ? " - $pageTitle" : '' ?></title>

    <link href="https://fonts.googleapis.com/css2?family=Noto+Serif&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="styles.css">



</head>
<body>

<main class="main-content">


    <?= $content ?>

</main>
</body>
</html>
