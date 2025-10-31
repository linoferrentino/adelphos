<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Odan\Session\SessionInterface;

final class UserDoRegisterStepTwoAction
{

    public function __construct(
	    private PhpRenderer $renderer,
	private SessionInterface $session
    ) {}


    public function __invoke(Request $request, Response $response, array $args): Response
    {

	    $params = (array)$request->getParsedBody();

	    $json_params = json_encode($params);

	    // I put the params in the session, this is the step two, so there are only these
	    $this->session->set(\SES_REGISTRATION_KEY, $json_params);

	    $bread_crumbs = [
		    'Home' => '/',
		    'name/family' => '/user/register',
		    'currency' => '/user/do_register_step_2'
	    ];


	    $attributes = [
		    'help_page' => 'create_user_step_two',
		    'bread_crumbs' => $bread_crumbs,
		    'json_params' => $json_params
	    ];
	    
            $attributes = make_bag_parameters($attributes, $this->session);

	    return $this->renderer->render($response, 'user/create_step_two.html.php', $attributes);

    }
}

?>
