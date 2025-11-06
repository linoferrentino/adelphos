<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Odan\Session\SessionInterface;
use App\Backend\AdelphosBE;

final class UserDoRegisterStepTwoAction
{

    public function __construct(
	    private PhpRenderer $renderer,
	    private SessionInterface $session,
	    private AdelphosBE $backend
    ) {}

    // a function that gets the currencies defined into the system: this is directly linked to
    // the database.


    public function __invoke(Request $request, Response $response, array $args): Response
    {

	    // let's get the currencies!
	    $currencies = $this->backend->get_currencies()->currencies;


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
		    'json_params' => $json_params,
		    'currencies' => $currencies
	    ];
	    
            $attributes = make_bag_parameters($attributes, $this->session);

	    return $this->renderer->render($response, 'user/create_step_two.html.php', $attributes);

    }
}

?>
