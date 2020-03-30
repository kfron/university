package com.coldproject.DwarvenTreasure.Entity;

import java.awt.Graphics2D;
import java.awt.Rectangle;

import com.coldproject.DwarvenTreasure.Map.Map;
import com.coldproject.DwarvenTreasure.Map.Tile;

public abstract class Entity
{
	// pozycja
	protected int x;
	protected int y;
	protected int xdest;
	protected int ydest;
	protected int rowTile;
	protected int colTile;

	// zmiana polozenia

	protected boolean isMoving;
	protected boolean left;
	protected boolean right;
	protected boolean up;
	protected boolean down;

	// mapa
	protected Map map;
	protected int tileSize;
	protected int xmap;
	protected int ymap;


	protected int moveSpeed;

	// animacje
	protected Animation animation;
	protected int currAnimation;

	public Entity(Map map)
	{
		this.map = map;
		tileSize = map.getTileSize();
		animation = new Animation();
	}

	public int getx()
	{
		return x;
	}

	public int gety()
	{
		return y;
	}

	public int getRow()
	{
		return rowTile;
	}

	public int getCol()
	{
		return colTile;
	}

	public void setPosition(int i1, int i2)
	{
		x = i1;
		y = i2;
		xdest = x;
		ydest = y;
	}

	public void setMapPosition()
	{
		xmap = map.getx();
		ymap = map.gety();
	}

	public void setTilePosition(int i1, int i2)
	{
		x = i1 * tileSize + tileSize / 2;
		y = i2 * tileSize + tileSize / 2;
		xdest = x;
		ydest = y;
	}

	public void setLeft()
	{
		if(isMoving) return;
		right = false;
		up = false;
		down = false;
		left = true;
		isMoving = validateNextPosition();
	}

	public void setRight()
	{
		if(isMoving) return;
		right = true;
		up = false;
		down = false;
		left = false;
		isMoving = validateNextPosition();
	}

	public void setUp()
	{
		if(isMoving) return;
		up = true;
		down = false;
		left = false;
		right = false;
		isMoving = validateNextPosition();
	}

	public void setDown()
	{
		if(isMoving) return;
		down = true;
		left = false;
		right = false;
		up = false;
		isMoving = validateNextPosition();
	}

	public boolean intersects(Entity o)
	{
		return getRectangle().intersects(o.getRectangle());
	}

	public Rectangle getRectangle()
	{
		return new Rectangle(x, y, 12, 12);
	}

	// sprawdzenie docelowej pozycji - czy entity może sie tam przesunąć

	public boolean validateNextPosition()
	{
		if(isMoving) return true;

		rowTile = y / tileSize;
		colTile = x / tileSize;

		if(left)
		{
			if(colTile == 0 || map.getType(rowTile, colTile - 1) == Tile.BLOCKED)
			{
				return false;
			}
			else
			{
				xdest = x - tileSize;
			}
		}

		if(right)
		{
			if(colTile == map.getColsNumber() || map.getType(rowTile, colTile + 1) == Tile.BLOCKED)
			{
				return false;
			}
			else
			{
				xdest = x + tileSize;
			}
		}

		if(up)
		{
			if(rowTile == 0 || map.getType(rowTile - 1, colTile) == Tile.BLOCKED)
			{
				return false;
			}

			else
			{
				ydest = y - tileSize;
			}
		}

		if(down)
		{


			if(rowTile == map.getRowsNumber() -1 || map.getType(rowTile + 1, colTile) == Tile.BLOCKED)
			{
				return false;
			}

			else
			{
				ydest = y + tileSize;
			}
		}

		return true;
	}

	public void getNextPosition()
	{
		if(up && y > ydest) y -= moveSpeed;
		else up = false;
		if(up && y < ydest) y = ydest;

		if(down && y < ydest) y += moveSpeed;
		else down = false;
		if(down && y > ydest) y = ydest;

		if(left && x > xdest) x -= moveSpeed;
		else left = false;
		if(left && x < xdest) x = xdest;

		if(right && x < xdest) x += moveSpeed;
		else right = false;
		if(right && x > xdest) x = xdest;
	}

	public void update()
	{
		if(isMoving) getNextPosition();

		if(x == xdest && y == ydest)
		{
			left = right = up = down = isMoving = false;
			rowTile = y / tileSize;
			colTile = x / tileSize;
		}

		animation.update();
	}

	public void draw(Graphics2D g)
	{
		setMapPosition();
		g.drawImage(animation.getImage(), x + xmap - 16 / 2, y + ymap - 16 / 2, null);
	}


}