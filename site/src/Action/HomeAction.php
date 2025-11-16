<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;
use Odan\Session\FlashInterface;

final class HomeAction
{


    public function __construct(
	    private PhpRenderer $renderer,
	    private LoggerInterface $logger,
	    private SessionInterface $session
    ) {}

    public function __invoke(Request $request, Response $response): Response
    {

	    // is there already a user? If yes you should render the Login menu
	    // otherwise put the register action.
	    if ($this->session->get('user_id') !== null) {
	    	return $this->renderer->render($response, 'home/home_logged.html.php', $attributes);
	    }

	    // menu items are dependent from the state of the application.


	    $menu_items = [
		    "<a href=\"/user/login\">Login</a>",
		    "<a href=\"/user/register\">Register</a>",
		    "<a href=\"/user/got_invite\">Got invite</a>"
	    ];
	    
	    // Rendering the home.html.php template
	    $attributes = [

		    'help_page' => '',
		    'menu_items' => $menu_items
	    
	    ];

	    $attributes = make_bag_parameters($attributes, $this->session);

	    return $this->renderer->render($response, 'home/home.html.php', $attributes);
    }

    
}

?>
