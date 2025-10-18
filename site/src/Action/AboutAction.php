<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;

final class AboutAction
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
		    'help_page' => 'about',
	    ];
	    
	    return $this->renderer->render($response, 'general/about.html.php', $attributes);

    }
}

?>
