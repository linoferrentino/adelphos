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
use App\Backend\AdelphosBE;
use App\Backend\AdelphosBE\instantiate_back_end;
use App\Backend\AdelphosBE\mock\MockAdelphosBe;
use Symfony\Component\Mailer\MailerInterface;
use Symfony\Component\EventDispatcher\EventDispatcherInterface;
use Symfony\Component\Mailer\Mailer;
use Symfony\Component\Mailer\Transport;
use Nyholm\Psr7\Factory\Psr17Factory;
use Symfony\Component\EventDispatcher\EventDispatcher;
use Symfony\Component\Mailer\EventListener\EnvelopeListener;
use Symfony\Component\Mailer\EventListener\MessageListener;
use Symfony\Component\Mailer\EventListener\MessageLoggerListener;

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

    AdelphosBE::class => function (ContainerInterface $container) {
        $options = $container->get('settings')['adelphos-backend'];
	#return \App\Backend\instantiate_back_end($options['backend-instance']);
	return instantiate_back_end($options['backend-instance']);
    },


    MailerInterface::class => function (ContainerInterface $container) {
        $settings = $container->get('settings')['mailer'];
        $dsn = $settings['dsn'];
        $eventDispatcher = $container->get(EventDispatcherInterface::class);
        return new Mailer(Transport::fromDsn($dsn, $eventDispatcher));
    },

    // Event dispatcher for mailer. Required to retrieve emails when testing.
    EventDispatcherInterface::class => function () {
        $eventDispatcher = new EventDispatcher();
        $eventDispatcher->addSubscriber(new MessageListener());
        $eventDispatcher->addSubscriber(new EnvelopeListener());
        $eventDispatcher->addSubscriber(new MessageLoggerListener());

        return $eventDispatcher;
    }


/*,

    ResponseFactoryInterface::class => function (ContainerInterface $container) {
        return $container->get(Psr17Factory::class);
    },
     */


];


?>
