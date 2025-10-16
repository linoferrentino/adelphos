<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;

final class DoPayAction
{


    public function __construct(
	    private PhpRenderer $renderer,
	    private LoggerInterface $logger,
	    private SessionInterface $session
    ) {}


    public function __invoke(Request $request, Response $response, array $args): Response
    {

	    // Attributes array passed to the template renderer via the third parameter of the render method
	    $attributes = [
		    // Attributes that will be accessible as variables in the template with the key being the variable name
		    'pageTitle' => 'Home', // $pageTitle -> 'Home'
		    'name' => 'Do pay for ' . strval($args['id'])
	    ];
	    // Attributes can also be added individually with the addAttribute() method
	    $this->renderer->addAttribute('appName', 'Slim App');

	    // Rendering the home.html.php template
	    return $this->renderer->render($response, 'home/home.html.php', $attributes);

    }
}

?>
