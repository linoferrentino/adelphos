<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;

final class UserDoRegisterAction
{


    public function __construct(
	    private PhpRenderer $renderer,
	    private LoggerInterface $logger,
	    private SessionInterface $session
    ) {}


    public function __invoke(Request $request, Response $response, array $args): Response
    {


	    // Get all POST parameters
	    $params = (array)$request->getParsedBody();

	    // Get a single POST parameter
	    //$userid = $params['userid'] ?? 'n/a';
	    //$password = $params['password'] ?? 'n/a';


	    // Attributes array passed to the template renderer via the third parameter of the render method
	    $attributes = [
		    'help_page' => '',
	            'pars' => print_r($params, true)
	    ];
	    
	    // Rendering the home.html.php template
	    return $this->renderer->render($response, 'home/home.html.php', $attributes);

    }
}

?>
