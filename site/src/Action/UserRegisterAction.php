<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;


final class UserRegisterAction 
{

	public function __construct(
		private PhpRenderer $renderer,
		private LoggerInterface $logger,
		private SessionInterface $session
	) {}

	public function __invoke(Request $request, Response $response): Response
	{


		/* here we remove session data for the user. */
		$this->session->delete(\SES_REGISTRATION_KEY);

		$attributes = [
			'help_page' => 'create_user'
		];

		// Rendering the home.html.php template
		return $this->renderer->render($response, 'user/register.html.php', $attributes);

	}	
}
