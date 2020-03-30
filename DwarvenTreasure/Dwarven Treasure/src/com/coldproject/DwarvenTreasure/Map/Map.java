package com.coldproject.DwarvenTreasure.Map;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import com.coldproject.DwarvenTreasure.Main.GamePanel;

public class Map
{
	private int rowsToDraw;
	private int colsToDraw;
	private int rowOffset;
	private int colOffset;

	private int width;
	private int height;
	private int rows;
	private int cols;

	private int xmin;
	private int xmax;
	private int ymin;
	private int ymax;

	private int tileSize;
	private int tilesAcross;
	private Tile[][] tiles;
	private int[][] map;

	private int x;
	private int y;
	private int xdest;
	private int ydest;
	private boolean isMoving;
	private int speed;

	private BufferedImage tileset;
	public Map()
	{
		tileSize = 16;
		rowsToDraw = GamePanel.HEIGHT / tileSize + 2;
		colsToDraw = GamePanel.WIDTH / tileSize + 2;
		speed = 4;

	}

	public void loadTiles(String s)
	{
		try
		{
			tileset = ImageIO.read(getClass().getResourceAsStream(s));

			tilesAcross = tileset.getWidth() / tileSize;

			tiles = new Tile[2][tilesAcross];

			BufferedImage subimage;
			for(int i = 0; i < tilesAcross; i++)
			{
				subimage = tileset.getSubimage(i * tileSize, 0, tileSize, tileSize);
				tiles[0][i] = new Tile(subimage, Tile.FREE);
				subimage = tileset.getSubimage(i * tileSize, tileSize, tileSize, tileSize);
				tiles[1][i] = new Tile(subimage, Tile.BLOCKED);
			}

		}catch(Exception e)
		{
			e.printStackTrace();
			System.exit(0);
		}
	}

	public void loadMap(String s)
	{
		try
		{
			InputStream in = getClass().getResourceAsStream(s);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));

			cols = Integer.parseInt(br.readLine());
			rows = Integer.parseInt(br.readLine());
			map = new int[rows][cols];
			width = cols * tileSize;
			height = rows * tileSize;

			xmin = GamePanel.WIDTH - width;
			xmin = -width;
			xmax = 0;
			ymin = GamePanel.HEIGHT - height;
			ymin = -height;
			ymax = 0;

			String delimiters = " ";
			for(int row = 0; row < rows; row++)
			{
				String line = br.readLine();
				String[] tokens = line.split(delimiters);
				for(int col = 0; col < cols; col++)  
				{
					map[row][col] = Integer.parseInt(tokens[col]);
				}
			}

		}
		catch(Exception e)
		{
			e.printStackTrace();
			System.exit(0);
		}
	}

	public void draw(Graphics2D g)
	{
		for(int row = rowOffset; row < rowOffset + rowsToDraw; row++)
		{
			if(row >= rows) break;
			for(int col = colOffset; col < colOffset + colsToDraw; col++)
			{
				int rowcol = map[row][col];
				int r = rowcol / tilesAcross;
				int c = rowcol % tilesAcross;

				g.drawImage(tiles[r][c].getImg(), x + col * tileSize, y + row * tileSize, null);
			}
		}
	}

	public int getIndex(int row, int col)
	{
		return map[row][col];
	}

	public void setTile(int row, int col, int index)
	{
		map[row][col] = index;
	}

	public void update()
	{
		if(x < xdest) {
			x += speed;
			if(x > xdest) {
				x = xdest;
			}
		}
		if(x > xdest) {
			x -= speed;
			if(x < xdest) {
				x = xdest;
			}
		}
		if(y < ydest) {
			y += speed;
			if(y > ydest) {
				y = ydest;
			}
		}
		if(y > ydest) {
			y -= speed;
			if(y < ydest) {
				y = ydest;
			}
		}

		fixBounds();

		colOffset = -this.x / tileSize;
		rowOffset = -this.y / tileSize;

		if(x != xdest || y != ydest) isMoving = true;
		else isMoving = false;
	}

	public void fixBounds()
	{
		if(x < xmin) x = xmin;
		if(y < ymin) y = ymin;
		if(x > xmax) x = xmax;
		if(y > ymax) y = ymax;
	}

	public void setInitialPosition(int x, int y)
	{
		this.x = x;
		this.y = y;
	}

	public void setPosition(int x, int y)
	{
		xdest = x;
		ydest = y;
	}

	public boolean isMoving()
	{
		return isMoving;
	}

	public int getTileSize()
	{
		return tileSize;
	}

	public int getx()
	{
		return x;
	}

	public int gety()
	{
		return y;
	}


	public int getType(int row, int col) {
		int rc = map[row][col];
		int r = rc / tilesAcross;
		int c = rc % tilesAcross;
		return tiles[r][c].getType();
	}

	public int getRowsNumber()
	{
		return rows;
	}

	public int getColsNumber()
	{
		return cols;
	}
}