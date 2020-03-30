package com.coldproject.DwarvenTreasure.Management;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import javax.imageio.ImageIO;

public class Graphics
{
	public static BufferedImage[][] font = load("font.png", 8, 8);
	
	public static BufferedImage[][] BAR = load("bar.png", 128, 16);


	public static BufferedImage[][] PLAYER = load("player.png", 16, 16);

	public static BufferedImage[][] GOLD = load("gold.png", 16, 16);


	public static BufferedImage[][] GRAAL = load("graal.png", 16, 16);


	public static BufferedImage[][] ITEM = load("items.png", 16, 16);

	public static BufferedImage[][] load(String s, int w, int h)
	{
		BufferedImage[][] ret;
		try
		{
			BufferedImage sprites = ImageIO.read(Graphics.class.getResourceAsStream(s));
			int width = sprites.getWidth() / w;
			int height = sprites.getHeight() / h;
			ret = new BufferedImage[height][width];
			for(int i = 0; i < height; i++)
			{
				for(int j = 0; j < width; j++)
				{
					ret[i][j] = sprites.getSubimage(j*w, i*h, w, h);
				}
			}

			return ret;

		}catch(Exception e)
		{
			e.printStackTrace();
			System.out.println("ERROR WHILE LOADING GRAPHICS");
			System.exit(0);
		}
		return null;
	}

	public static void drawString(Graphics2D g, String s, int x, int y)
	{
		s = s.toUpperCase();
		for(int i = 0; i < s.length(); i++) {
			char c = s.charAt(i);
			if(c == 47) c = 36; // slash
			if(c == 58) c = 37; // colon
			if(c == 32) c = 38; // space
			if(c >= 65 && c <= 90) c -= 65; // litery
			if(c >= 48 && c <= 57) c -= 22; // cyfry
			int row = c / font[0].length;
			int col = c % font[0].length;
			g.drawImage(font[row][col], x + 8 * i, y, null);
		}
	}
}