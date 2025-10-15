<?php

use Psr\Container\ContainerInterface;
use Slim\App;
use Slim\Factory\AppFactory;
use Monolog\Logger;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Monolog\Handler\RotatingFileHandler;
use Monolog\Formatter\LineFormatter;
use Odan\Session\SessionInterface;
use Odan\Session\SessionManagerInterface;
use Odan\Session\PhpSession;

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


    PhpRenderer::class => function (ContainerInterface $container) {

	$settings = $container->get('settings')['renderer'];
        return new PhpRenderer($settings['path']);

    },


    LoggerInterface::class => function (ContainerInterface $container) {
	    $loggerSettings = $container->get('settings')['logger'];

	    $logger = new Logger('app');

	    // When testing, 'test' value is true which means the monolog test handler should be used
	    if (isset($loggerSettings['test']) && $loggerSettings['test'] === true) {
		    return $logger->pushHandler(new \Monolog\Handler\TestHandler());
	    }

	    // Instantiate logger with rotating file handler
	    $filename = sprintf('%s/app.log', $loggerSettings['path']);
	    $level = $loggerSettings['level'];
	    // With the RotatingFileHandler, a new log file is created every day
	    $rotatingFileHandler = new RotatingFileHandler($filename, 0, $level, true, 0777);
	    // The last "true" here tells monolog to remove empty []'s
	    $rotatingFileHandler->setFormatter(new LineFormatter(null, 'Y-m-d H:i:s', false, true));
	    return $logger->pushHandler($rotatingFileHandler);
    },

    SessionManagerInterface::class => function (ContainerInterface $container) {
        return $container->get(SessionInterface::class);
    },

    SessionInterface::class => function (ContainerInterface $container) {
        $options = $container->get('settings')['session'];
        return new PhpSession($options);
    },


    

];


?>
