<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;

final class LoginAction
{


    public function __construct(
	    private PhpRenderer $renderer,
	    private LoggerInterface $logger,
	    private SessionInterface $session
    ) {}


    // Just a private function to authenticate, for the demo
    private function _do_authenticate($userid) : bool
    {
	    if ( $userid == 'alice.smith@manhattan_01') {
		    return true;
	    } else {
		    return false;
	    }
    }




    public function __invoke_new(Request $request, Response $response): Response
    {
	    // I take the parameters
	    $params = (array)$request->getParsedBody();

	    $json_params = json_encode($params);

	    // I put the params in the session, this is the step two, so there are only these
	    $this->session->set(\SES_REGISTRATION_KEY, $json_params);

	    $flash = $this->session->getFlash();

	    $userid = $params['userid'];

	    $res = $this->_do_authenticate($userid);

	    if ($res === true) {

	    	$flash->add('success', 'login successful');

        	return $response->withStatus(302)->withHeader('Location', '/summary');
	    } 
	    
	    
	    $flash->add('error', 'wrong credentials');
	    

	    $attributes = [
		    'help_page' => '',
		    'session' => $this->session,
		    'flash' => $flash
	    ];
	

	    return $this->renderer->render($response, 'home/home.html.php', $attributes);

    }

    public function __invoke(Request $request, Response $response): Response
    {
	    /*
	    if ($this->session->has('ntimes')) {
		    $ntimes = $this->session->get('ntimes');
		    $new_val = intval($ntimes) + 1; 
	    } else {
		    $new_val = 1;
	    }
            $this->session->set('ntimes', strval($new_val));
	     */



	    $bread_crumbs = [
		    'Home' => '/',
		    'login' => '/user/login'
	    ];


	    $attributes = [
		    'help_page' => 'login_help',
		    'bread_crumbs' => $bread_crumbs
	    ];

	    $attributes = make_bag_parameters($attributes, $this->session);


	    // Rendering the home.html.php template
	    return $this->renderer->render($response, 'user/login_user.html.php', $attributes);

    }
}

?>
