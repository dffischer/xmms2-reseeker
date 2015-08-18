# Maintainer: XZS <d dot f dot fischer at web dot de>
pkgname=xmms2-reseeker
pkgver=r0
pkgrel=1
pkgdesc="A tiny xmms2 client that skips back a few seconds every time a song resumes from pause."
arch=('i686' 'x86_64')
url="https://github.com/dffischer/xmms2-reseeker"
license=('GPLv3')
depends=('xmms2')
makedepends=('waf')
install=xmms2-reseeker.install

# template input; name=git

build() {
	cd "$_gitname"
	waf --prefix=/usr configure build
}

package() {
	cd "$_gitname"
	waf install --destdir="$pkgdir/"
}
