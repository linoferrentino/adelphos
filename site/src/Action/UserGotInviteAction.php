<?php

namespace App\Action;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;
use Psr\Log\LoggerInterface;
use Odan\Session\SessionInterface;
use Odan\Session\FlashInterface;

final class UserGotInviteAction
{


	public function __construct(
		private PhpRenderer $renderer,
		private SessionInterface $session
	) {}

	public function __invoke(Request $request, Response $response): Response
	{


		$attributes = [];


		// if there is already a user, then it is useless
		// to have an invite.
		if ($this->session->get('user_id') !== null) {
			return $this->renderer->render($response, 'home/home_logged.html.php', $attributes);
		}


		return $this->renderer->render($response, 'user/got_invite.html.php', $attributes);
	}

}
