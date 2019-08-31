class SdlGfx < Formula
  desc "Graphics drawing primitives and other support functions"
  homepage "http://www.ferzkopp.net/joomla/content/view/19/14/"
  url "http://www.ferzkopp.net/Software/SDL_gfx-2.0/SDL_gfx-2.0.26.tar.gz"
  sha256 "7ceb4ffb6fc63ffba5f1290572db43d74386cd0781c123bc912da50d34945446"

  depends_on "sdl"

  def install
    system "./configure", "--disable-dependency-tracking",
                          "--prefix=#{prefix}",
                          "--disable-sdltest"
    system "make", "install"
  end
end
