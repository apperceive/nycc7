<?php
$a = array();

$sql = "SELECT uid, pid FROM profile ORDER BY uid ASC, pid ASC";
$q = db_query($sql);
foreach ($q as $r) {
  if (!array_key_exists($r->uid, $a))
    $a[$r->uid] = $r->pid;
  else {
    if ($a[$r->uid] != $r->pid)
      db_delete('profile')->condition('pid', $r->pid)->execute();
  }
}