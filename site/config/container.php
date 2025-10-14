<?php

use Psr\Container\ContainerInterface;
use Slim\App;
use Slim\Factory\AppFactory;
use Monolog\Logger;
use Monolog\Handler\StreamHandler;

return [
    'settings' => function () {
        return require __DIR__ . '/settings.php';
    },

    App::class => function (ContainerInterface $container) {
        $app = AppFactory::createFromContainer($container);

        // Register routes
        (require __DIR__ . '/routes.php')($app);

        // Register middleware
        (require __DIR__ . '/middleware.php')($app);

	#$app->setBasePath("/site/public");

	$app->addErrorMiddleware(true, true, true); // Enable error handling middleware

        return $app;
    },

    'logger' => function(ContainerInterface $c) {
	    $logger = new \Monolog\Logger('my_logger');
	    $file_handler = new \Monolog\Handler\StreamHandler('../logs/app.log');
	    $logger->pushHandler($file_handler);
	    return $logger;
    }

];


?>
