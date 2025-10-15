
<?php

use Slim\App;

return function (App $app) {
    $app->get('/', \App\Action\HomeAction::class);

	$app->get('/login', \App\Action\LoginAction::class);

};


?>
