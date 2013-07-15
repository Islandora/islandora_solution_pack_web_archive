<?php

/**
 * @file
 * This is the template file for the object page for web archives
 *
 * @TODO: add documentation about file and available variables
 */
?>

<div class="islandora-web-archive-object islandora">
  <div class="islandora-web-archive-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="islandora-web-archive-content">
        <?php print $islandora_content; ?>
      </div>
    <?php endif; ?>
  <div class="islandora-web-archive-sidebar">
    <?php if (!empty($dc_array['dc:description']['value'])): ?>
      <h2><?php print $dc_array['dc:description']['label']; ?></h2>
      <p><?php print $dc_array['dc:description']['value']; ?></p>
    <?php endif; ?>
    <div>
      <h2><?php print t('Download'); ?></h2>
        <ul>
          <?php $result = drupal_http_request($islandora_warc); if(in_array($result->code, array(-1002, 200))): ?>
            <li>Warc: <?php print $islandora_warc; ?>
          <?php endif; ?>
          <?php $result = drupal_http_request($islandora_pdf); if(in_array($result->code, array(-1002, 200))): ?>  
            <li>PDF: <?php print $islandora_pdf; ?>
          <?php endif; ?>
          <?php $result = drupal_http_request($islandora_png); if(in_array($result->code, array(-1002, 200))): ?>
            <li>Screenshot: <?php print $islandora_png; ?>
          <?php endif; ?>
        </ul>
    </div>
    <?php if ($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
            <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
  </div>
  </div>
  <fieldset class="collapsible collapsed islandora-web-archive-metadata">
  <legend><span class="fieldset-legend"><?php print t('Details'); ?></span></legend>
    <div class="fieldset-wrapper">
      <dl class="islandora-inline-metadata islandora-web-archive-fields">
        <?php $row_field = 0; ?>
        <?php foreach($dc_array as $key => $value): ?>
          <dt class="<?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">
            <?php print $value['label']; ?>
          </dt>
          <dd class="<?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">
            <?php print $value['value']; ?>
          </dd>
          <?php $row_field++; ?>
        <?php endforeach; ?>
      </dl>
    </div>
  </fieldset>
</div>
