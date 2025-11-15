<?php

use Slim\App;
use SlimErrorRenderer\Middleware\ExceptionHandlingMiddleware;
use App\Application\Middleware\ValidationExceptionMiddleware;
use SlimErrorRenderer\Middleware\NonFatalErrorHandlingMiddleware;

return function (App $app) {
	
	// Parse json, form data and xml
	$app->addBodyParsingMiddleware();

	$app->add(Odan\Session\Middleware\SessionStartMiddleware::class);
	
	// Add the Slim built-in routing middleware
	$app->addRoutingMiddleware();


	//$app->add(\App\Application\Middleware\ValidationExceptionMiddleware::class);
	$app->add(new ValidationExceptionMiddleware($app->getResponseFactory()));

	// Handle exceptions
	//$app->addErrorMiddleware(true, true, true);

	
	$app->add(NonFatalErrorHandlingMiddleware::class); 
	

	$app->add(ExceptionHandlingMiddleware::class);
};

?>
