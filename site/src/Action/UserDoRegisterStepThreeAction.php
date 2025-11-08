<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Odan\Session\SessionInterface;

final class UserDoRegisterStepThreeAction
{

	public function __construct(
		private PhpRenderer $renderer,
		private SessionInterface $session
	) {}


    public function __invoke(Request $request, Response $response, array $args): Response
    {

	    $params = (array)$request->getParsedBody();

	    $old_params_json = json_decode($this->session->get(\SES_REGISTRATION_KEY), true);

	    $currencies = json_decode($this->session->get(\SES_CURRENCIES), true);

	    $params = array_merge($params, $old_params_json);

	    //$json_params = json_encode($params);

	    $bread_crumbs = [
		    'Home' => '/',
		    'name/family' => '/user/register',
		    'currency' => '/user/do_register_step_2',
		    'l₀ trust levels' => '/user/do_register_step_3'
	    ];


	    $attributes = [
		    'help_page' => 'money-in-adelphos',
		    'bread_crumbs' => $bread_crumbs,
		    'params' => $params,
		    'currencies' => $currencies
	    ];
	    
            $attributes = make_bag_parameters($attributes, $this->session);

	    
	    return $this->renderer->render($response, 'user/create_step_three.html.php', $attributes);

    }
}

?>
