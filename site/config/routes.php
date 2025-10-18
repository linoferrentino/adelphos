
<?php

use Slim\App;

return function (App $app) {

	$app->get('/', \App\Action\HomeAction::class);

	$app->get('/about', \App\Action\AboutAction::class);

	$app->get('/login', \App\Action\LoginAction::class);
	$app->get('/login/login/to', \App\Action\LoginAction::class);


	$app->get('/user/register', \App\Action\UserRegisterAction::class);

	$app->post('/user/do_register', \App\Action\UserDoRegisterAction::class);

	$app->get('/dopay/{id}', \App\Action\DoPayAction::class);

};


?>
