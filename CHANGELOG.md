# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.6.0] - 2016-09-30
### Added
 - New `cytometry` plugin.
 - New `fluorescence` plugin.
 - New `object-streamlines-generator` plugin.
 - New `diffusion-streamlines-python` plugin.

### Changed
 - Improved data exporting modules.
 - Objects and modules can be hidden by visualization layers.
 - `streamlines` plugin boundaries can be named.
 - Created separated repositories for core library and plugins.
 - Plugins failed to load don't terminate application.
 - Fixed `stochastic reactions` plugin rate - relative to time unit instead of iteration + can handle `/s` suffix.
 - CLI: Fixed non-working <kbd>Ctrl</kbd> + <kbd>C</kbd> on Windows.

[0.6.0]: https://github.com/georgievlab/CeCe/compare/v0.5...v0.6.0
