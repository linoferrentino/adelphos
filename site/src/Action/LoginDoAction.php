<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;

final class LoginDoAction
{

    public function __construct(
	    private PhpRenderer $renderer,
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

    public function __invoke(Request $request, Response $response): Response
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
	
	    $attributes = make_bag_parameters($attributes);

	    return $this->renderer->render($response, 'home/home.html.php', $attributes);

    }


}


?>
