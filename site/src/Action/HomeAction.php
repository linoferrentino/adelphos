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


	    $menu_items = [
		    "<a href=\"/user/register\">Register</a>",
	    ];

	    
	    // Rendering the home.html.php template
	    $attributes = [

		    'help_page' => '',

		    'menu_items' => $menu_items

	    
	    ];
	    return $this->renderer->render($response, 'home/home.html.php', $attributes);
    }

    public function invoke_OLD(Request $request, Response $response): Response
    {
	    if ($this->session->has('ntimes')) {
		    $ntimes = $this->session->get('ntimes');
		    $new_val = intval($ntimes) + 1; 
	    } else {
		    $new_val = 1;
	    }
            $this->session->set('ntimes', strval($new_val));

	    $this->logger->Info("calling the home action");

	    $socket = socket_create(AF_UNIX, SOCK_STREAM, 0);
	    if ($socket === false) {
		    $daemon_answer = socket_strerror(socket_last_error()); 
		    goto do_page;
	    }

	    $res = socket_connect($socket, "/tmp/adelphos");
	    if ($res === false) {
		    $daemon_answer = socket_strerror(socket_last_error()); 
		    goto do_page;
	    }

	    // Ok, now we send some data. 

	    $myhello = "42 the meaning of everything";
	    $res = socket_write($socket, $myhello);
	    if ($res === false) {
		    $daemon_answer = socket_strerror(socket_last_error()); 
		    goto do_page;
	    }

	    //Now we get the answer
	    $daemon_answer = socket_read($socket, 200);
	    if ($daemon_answer === false) {
		    $daemon_answer = socket_strerror(socket_last_error()); 
		    goto do_page;
	    }

	    //OK now I close
	    socket_close($socket);


	    // Attributes array passed to the template renderer via the third parameter of the render method
do_page:

	    $attributes = [
		    // Attributes that will be accessible as variables in the template with the key being the variable name
		    'pageTitle' => 'Home', // $pageTitle -> 'Home'
		    'name' => 'anonymous ' . strval($new_val) . ' daemon ' . $daemon_answer
	    ];
	    // Attributes can also be added individually with the addAttribute() method
	    $this->renderer->addAttribute('appName', 'Adelphos App');

	    // Rendering the home.html.php template
	    return $this->renderer->render($response, 'home/home.html.php', $attributes);

    }
}

?>
