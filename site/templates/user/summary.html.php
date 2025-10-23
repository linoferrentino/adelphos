<?php

 // Set layout
$this->setLayout('layout.html.php')

?>


<div class="summary-container">
  <h2>Welcome back, Lino</h2>
  <p class="login-info">Your last login was: <strong>October 22nd 2025</strong></p>

  <div class="flash-message flash-info">📨 You have <strong>3 unread messages</strong>.</div>
  <div class="flash-message flash-warning">💸 You owe <strong>$34</strong> in redeemable money to Bob next week.</div>
  <div class="flash-message flash-success">🧾 You can claim <strong>$10</strong> from Alice tomorrow.</div>

  <h3>Your current balances</h3>
  <table class="balance-table">
    <thead>
      <tr>
        <th>Level Balance</th>
        <th>Redeemable Money</th>
        <th>Unredeemable Money</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>Personal Balance</td>
        <td>$34</td>
        <td>−$21</td>
      </tr>
      <tr>
        <td>Your Family's Balance</td>
        <td>$55</td>
        <td>$33</td>
      </tr>
    </tbody>
  </table>
</div>
