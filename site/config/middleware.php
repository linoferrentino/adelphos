<?php

use Slim\App;

return function (App $app) {
	
	// Parse json, form data and xml
	$app->addBodyParsingMiddleware();

	$app->add(Odan\Session\Middleware\SessionStartMiddleware::class);
	
	// Add the Slim built-in routing middleware
	$app->addRoutingMiddleware();

	// Handle exceptions
	$app->addErrorMiddleware(true, true, true);
};

?>
