
<?php

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\App;

return function (App $app) {

	$app->get('/', \App\Action\HomeAction::class);

	$app->get('/about', \App\Action\AboutAction::class);

	$app->post('/login', \App\Action\LoginAction::class);

	$app->get('/summary', \App\Action\SummaryAction::class);

	$app->get('/user/register', \App\Action\UserRegisterAction::class);

	/* these are the registration steps, the wizard. */
	$app->post('/user/do_register', \App\Action\UserDoRegisterAction::class);
	$app->post('/user/do_register_step_2', \App\Action\UserDoRegisterStepTwoAction::class);
	$app->get('/user/do_register_step_2', \App\Action\UserDoRegisterStepTwoAction::class);
	$app->post('/user/do_register_step_3', \App\Action\UserDoRegisterStepThreeAction::class);

	/* a user gets an invitation to join a family, level zero. */
	$app->get('/user/got_invite', \App\Action\UserGotInviteAction::class);



	$app->get('/params', function (Request $request, Response $response) {
		$response->getBody()->write('Hello World');
		return $response;
	});

};


?>
