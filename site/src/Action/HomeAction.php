<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;

final class HomeAction
{


	public function __construct(
		private PhpRenderer $renderer,
	) {}


    public function __invoke(Request $request, Response $response): Response
    {
        //$response->getBody()->write('Hello, LINO');
        //return $response;

	//$this->get("logger")->addInfo('Something interesting happened');
	//var_dump($this->$logger);
	//
	//



	    // Attributes array passed to the template renderer via the third parameter of the render method
	    $attributes = [
		    // Attributes that will be accessible as variables in the template with the key being the variable name
		    'pageTitle' => 'Home', // $pageTitle -> 'Home'
		    'name' => 'John Doe'
	    ];
	    // Attributes can also be added individually with the addAttribute() method
	    $this->renderer->addAttribute('appName', 'Slim App');

	    // Rendering the home.html.php template
	    return $this->renderer->render($response, 'home/home.html.php', $attributes);




	    /*
	$renderer = new PhpRenderer(__DIR__ . '/../templates');

	$viewData = [
		'name' => 'John',
	];

	return $renderer->render($response, 'hello.php', $viewData);
	     */



    }
}

?>
