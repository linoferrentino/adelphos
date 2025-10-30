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

	public function __invoke(Request $request, Response $response, array $args): Response
	{

		$queryParams = $request->getQueryParams();

		$query_string = $request->getUri()->getQuery();

		$invite_code = $queryParams['invite_code'] ?? null;

		$headers = $request->getHeaders();

		$bag = [
			'date' => date(DATE_RFC2822),
			'query_params' => $queryParams,
			'query_string' => $query_string,
			'invite_code' => $invite_code,
			'get' => $_GET,
			'super_args' => $args,
			'headers' => $headers
		];


		$attributes = [
			'bag' => $bag
		];


		// if there is already a user, then it is useless
		// to have an invite.
		if ($this->session->get('user_id') !== null) {
			return $this->renderer->render($response, 'home/home_logged.html.php', $attributes);
		}


		return $this->renderer->render($response, 'user/got_invite.html.php', $attributes);
	}

}
