<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;

final class UserDoRegisterStepThreeAction
{

    public function __construct(
	    private PhpRenderer $renderer,
    ) {}


    public function __invoke(Request $request, Response $response, array $args): Response
    {

	    $params = (array)$request->getParsedBody();

	    $attributes = [
		    'help_page' => 'create_user_step_three',
	            'pars' => print_r($params, true)
	    ];
	    
	    return $this->renderer->render($response, 'user/create_step_three.html.php', $attributes);

    }
}

?>
