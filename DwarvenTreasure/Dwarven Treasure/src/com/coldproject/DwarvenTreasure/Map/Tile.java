package com.coldproject.DwarvenTreasure.Map;

import java.awt.image.BufferedImage;

public class Tile
{
	private BufferedImage img;
	private int type;

	public static final int FREE = 0;
	public static final int BLOCKED = 1;

	public Tile(BufferedImage img, int type)
	{
		this.img = img;
		this.type = type;
	}

	public BufferedImage getImg()
	{
		return img;
	}

	public int getType()
	{
		return type;
	}
}